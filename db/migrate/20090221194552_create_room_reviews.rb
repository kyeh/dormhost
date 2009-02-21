class CreateRoomReviews < ActiveRecord::Migration
  def self.up
    create_table :room_reviews do |t|
      t.column  :room_id, :integer, :null => false
      t.column  :rating,  :integer
      t.column  :review,  :string

      t.timestamps
    end
  end

  def self.down
    drop_table :room_reviews
  end
end
