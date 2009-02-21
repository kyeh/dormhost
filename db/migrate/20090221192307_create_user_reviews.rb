class CreateUserReviews < ActiveRecord::Migration
  def self.up
    create_table :user_reviews do |t|
      t.column  :profile_id,  :integer, :null => false
      t.column  :user_id,     :integer, :null => false
      t.column  :rating,      :integer
      t.column  :review,      :string
      t.column  :status,      :boolean, :default => false
      t.column  :created_at,  :timestamp

      t.timestamps
    end
  end

  def self.down
    drop_table :user_reviews
  end
end
