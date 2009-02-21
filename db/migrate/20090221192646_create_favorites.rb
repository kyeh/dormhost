class CreateFavorites < ActiveRecord::Migration
  def self.up
    create_table :favorites do |t|
      t.column  :renter_id, :integer, :null => false
      t.column  :room_id,   :integer, :null => false

      t.timestamps
    end
  end

  def self.down
    drop_table :favorites
  end
end
