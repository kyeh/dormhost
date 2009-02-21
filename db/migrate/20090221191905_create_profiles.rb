class CreateProfiles < ActiveRecord::Migration
  def self.up
    create_table :profiles do |t|
      t.column  :user_id,       :integer, :null => false
      t.column  :college_id,    :integer, :null => false
      t.column  :image,         :string
      t.column  :birthdate,     :date
      t.column  :grad_year,     :date
      t.column  :phone,         :string, :limit => 20
      t.column  :living_style,  :string
      t.column  :bedtime,       :string
      t.column  :interests,     :string
      t.column  :smoker,        :boolean, :default => false

      t.timestamps
    end
  end

  def self.down
    drop_table :profiles
  end
end
